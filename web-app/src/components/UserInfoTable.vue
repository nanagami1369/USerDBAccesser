<template>
  <table id="user-data-table">
    <thead>
      <th class="id-column">ID</th>
      <th class="name-column">名前</th>
      <th class="password-column">パスワード</th>
      <th class="avail-column">状態</th>
      <th class="level-column">権限</th>
      <th class="update-button-column">更新</th>
      <th><!--余白--></th>
    </thead>
    <tbody>
      <SelectableTr
        v-for="user in userInfo"
        :key="user.id"
        :selectKey="user.id"
        @addKey="add"
        @removeKey="remove"
      >
        <td class="table-number">{{ user.id }}</td>
        <td>{{ user.name }}</td>
        <td>*******</td>
        <td class="table-other-content">{{ user.avail | availToString }}</td>
        <td class="table-other-content">{{ user.level | levelToString }}</td>
        <td class="table-other-content">
          <button id="update-button" type="button" @click.stop="update(user)">更新</button>
        </td>
        <td><!--余白--></td>
      </SelectableTr>
    </tbody>
  </table>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator'
import SelectableTr from '@/components/SelectableTr.vue'
import { User } from '@/model/User'
import { AvailToPrintString, LevelToPrintString } from '@/model/PrintString'

@Component({
  filters: {
    availToString: function(avail: boolean) {
      return AvailToPrintString(avail)
    },
    levelToString: function(level: string) {
      return LevelToPrintString(level)
    }
  },
  components: {
    SelectableTr
  }
})
export default class UserInfoTable extends Vue {
  @Prop() private userInfo!: User[]
  private slectedIds: Set<number> = new Set<number>()
  public add(key: string): void {
    this.slectedIds.add(Number(key))
    this.$emit('selectedChanged', this.slectedIds)
  }
  public remove(key: string): void {
    this.slectedIds.delete(Number(key))
    this.$emit('selectedChanged', this.slectedIds)
  }

  public update(updatedUser: User) {
    this.$emit('update', updatedUser)
    return
  }
}
</script>
<style scoped>
.id-column {
  width: 4rem;
}

.name-column {
  width: 30%;
}

.password-column {
  width: 7rem;
}

.avail-column {
  width: 3rem;
}

.level-column {
  width: 5rem;
}

.update-button-column {
  width: 7rem;
}

#update-button {
  flex-shrink: 0;
  font-size: 1.4em;
  padding: 0.2em 0.8em;
  margin: 0px 5px;
}
</style>
