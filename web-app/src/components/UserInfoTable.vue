<template>
  <table id="user-data-table">
    <thead>
      <th>ID</th>
      <th>名前</th>
      <th>パスワード</th>
      <th>状態</th>
      <th>権限</th>
    </thead>
    <tbody>
      <SelectableTr
        v-for="user in userInfo"
        :key="user.id"
        :selectKey="user.id"
        @addKey="add"
        @removeKey="remove"
      >
        <td>{{ user.id }}</td>
        <td>{{ user.name }}</td>
        <td>{{ user.pass }}</td>
        <td>{{ user.avail | availToString }}</td>
        <td>{{ user.level }}</td>
      </SelectableTr>
    </tbody>
  </table>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator'
import SelectableTr from '@/components/SelectableTr.vue'
import { User } from '@/model/User'

@Component({
  filters: {
    availToString: function(avail: boolean) {
      if (avail == true) {
        return '有効'
      } else {
        return '無効'
      }
    }
  },
  components: {
    SelectableTr
  }
})
export default class UserInfoTable extends Vue {
  @Prop() private userInfo!: User[]
  private slectedIds: number[] = []
  public add(key: string): void {
    this.slectedIds.push(Number(key))
    this.$emit('selectedChanged', this.slectedIds)
  }
  public remove(key: string): void {
    this.slectedIds = this.slectedIds.filter(x => x !== Number(key))
    this.$emit('selectedChanged', this.slectedIds)
  }
}
</script>
